libCppGUI: aspects
==================

A somewhat hard, but worthwhile way to achieve adaptability
----------------------------------------------------------

------------

### Introduction

Among the stated objectives of `libCppGUI` are for it to be highly adaptable without that adaptability incurring significant runtime costs.

The best way to achieve that would be a language feature called *aspects*, which is an advanced form of composition that C++, unfortunately, does not have.

What C++ does have, however, is multiple inheritance, plus rather advanced templating abilities. Those two features can be combined to achieve something that behaves quite a bit like aspects.

Before going into the technical details, it is perhaps a good idea to explain a bit more plainly what those "pseudo"-aspect are supposed to achieve. At the time of writing, the project is still largely experimental and the answer to that question is therefore still a bit up in the air. But essentially, aspects are intended to be used for two things:

1. to inject optional features

2. to inject specific implementations  of either exposed features or inner workings

So far, the project only has one of each category, though more are likely to come up as the project progresses:

- *Layouting* is a feature that the library treats as optional - though it is probably only optional in the sense that it could be done in some kind of designer application, and the resulting layout injected into the final application via constant data. (Although it is conceivable for the layouting to be done manually - in which case that aspect would then not be needed at all).

- The so-called *Updater* is the aspect of a widget that is responsible for making sure that any rendering done by a widget will actually be made visible to the user. (As hinted at in the [project introduction](../project_gui/project_gui.html#adapt_to_mo), this could actually become a rather complex matter, but for the present, the only existing implementation is really simple, since all it does is set a "dirty flag" that will trigger a redraw.)

Thus, what we are aiming for would ideally look something like this:

![Ideal aspect orientation](Aspects.svg?raw=true)

(The `Textbox` is of course just one of many widgets, chosen here as an example.) This diagram shows inheritance (or more accurately: specialization) as the traditional arrows, while the ties between a class and its aspects are shown as green straight lines.

### Implementation

When thinking about compositing functionality in C++, multiple inheritance is naturally the first thing that comes to mind: simply write the various building blocks as classes, and let the widget class inherit from the blocks that it needs. If that principle could be applied directly, this is what it would look like:

![Aspects via inheritance](Aspects_via_multiple_inheritance.svg?raw=true)

The problem is immediately apparent: an aspect derived from another (in this example, `Textbox__Layouter` derived from `Widget__Layouter`) can introduce the "diamond problem" - i.e., a class inherited more than once. This happens when the interface introduced by the base aspect (`Widget__Layouter`) is a) at least partly polymorphic (implying that overrides do or will exist), and b) must be publicly available. (This is of course the case for the Layouter aspect, which is responsible for placing visual elements in response to changes in size or style.)

Normally, the answer to these problems would be to use *virtual inheritance*. However, virtual inheritance has drawbacks: the base class lookup is done at runtime, and needs both an entry in the virtual method table and a lookup at runtime. Small costs, perhaps, nonetheless an alternative has been sought, and found.

That alternative consists in "sequencing" the inheritance graph - in other words, replacing multiple inheritance with "chained" single inheritance. This is made possible with the help of templates: instead of inheriting from several base classes, we turn those base classes into templates that each take a "parent" parameter, then inherit from a nested chain that contains all of them.

This principle, though syntactically less than beautiful, removes any runtime cost from accessing members across aspects.

![Aspect chaining](aspect-chaining.svg?raw=true)
