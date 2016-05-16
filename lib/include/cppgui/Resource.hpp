#pragma once

/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016 Hans-Peter Gygax

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

template<class Impl, typename SourceType, typename AdaptedType, class BackendType>
struct Resource_concept 
{
    using Implementation = typename Impl;

    void assign     (const SourceType &) { static_assert(false, "Resource<>::assign(): not implemented!"); }
    void translate  (BackendType *)      { static_assert(false, "Resource<>::translate(): not implemented!"); }
    void release    (BackendType *)      { static_assert(false, "Resource<>::release(): not implemented!"); }
    auto get        () -> AdaptedType    { static_assert(false, "Resource<>::get(): not implemented!"); return (AdaptedType) 0; }

    auto Resource_concept::operator = (const SourceType &resource) -> Resource_concept &
    {
        static_cast<Impl*>(this)->assign(resource);
        return *this;
    }
};

template<typename SourceType, typename AdaptedType, class BackendType, bool StaticTranslation>
struct Resource;

template<typename SourceType, typename AdaptedType, typename BackendType>
struct Resource<SourceType, AdaptedType, BackendType, true>: 
    public Resource_concept<Resource<SourceType, AdaptedType, BackendType, true>, SourceType, AdaptedType, BackendType>
{
    Resource::Resource(const SourceType &resource)
    {
        assign(resource);
    }

    void assign(const SourceType &resource)
    {
        _adapted = BackendType::adapt_resource(resource); // TODO: can detect a build time whether method is static ?
    }

    void translate(BackendType *) 
    {
        // No-op - already done at assign() time (just pass in a nullptr)
    }

    void release(BackendType *) 
    {
        BackendType::release_resource(_adapted); // if adapt_resource() is static, release_resource() must be too (TODO: check!)
    }

    auto get()
    {
        return _adapted;
    }

private:
    AdaptedType     _adapted;
};

template<typename SourceType, typename AdaptedType, typename BackendType>
struct Resource<SourceType, AdaptedType, BackendType, false>: 
    public Resource_concept<Resource<SourceType, AdaptedType, BackendType, false>, SourceType, AdaptedType, BackendType>
{
    using Resource_concept::Resource_concept;
    using Resource_concept::operator =;

    void assign(const SourceType &resource)
    {
        // TODO: implement debug-only check forbidding this when already translated

        _resource = resource;
    }

    void translate(BackendType *backend)
    {
        _adapted = backend->adapt_resource(_resource);
    }

    void release(BackendType *backend) 
    {
        BackendType::release_resource(_adapted);
    }

    auto get()
    {
        return _adapted;
    }

    /** Specific to this specialization.
     */
    auto source() const
    {
        return _resource;
    }

private:
    SourceType      _resource = {};
    AdaptedType     _adapted;
};

template<typename ...Members> struct Resource_struct: public Members... {};