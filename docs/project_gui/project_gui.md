<!--
author: Jean-Pierre Gygax
-->

A standardized, universally usable C++11/14 GUI library
=======================================================

Proposing a new effort
----------------------

by [Jean-Pierre Gygax](mailto:gygax@practicomp.ch), February 20, 2016

---------------------

### Motivation - why another C++ GUI when there are so many already ?

The truth of the matter is this: there aren't so many after all, and none of them are free of problems.

C++ is a unique language by its fundamental philosophy of "we don't pay for what we don't need", meaning not development costs but CPU cycles and memory footprint: C++ is all about not wasting resources at runtime. This is the big difference to everything else that is out there, and makes C++ usable under constraints where any other higher-level language - other than C - drops out of the race. C++ can be used to program embedded devices - there is little else that can make the same boast.

Now, there are a number of C++ GUI libraries out there that have pretty much all the features you could dream of, and more - Qt5 comes to mind -, but what those have in common is that they don't really follow the basic C++ tenet mentioned above. Qt is enormous in scope and possibilities, and is cross-platform, but it's very very far from being lightweight - not to mention that its licensing is "GPL or die" (well maybe not "die", but "reach pretty deep into your wallet").

There is also the fact that Qt and other "full" solutions have been in existence for quite some time, and though they have valiantly tried to adapt to a changing world, they remain products of their time in a sense: the design choices and tradeoffs that were good and right when these libraries were created are no longer the ones that best suit the landscape of today. The ubiquitousness of 3D-accelerated graphics hardware is certainly one of the things that have changed, but another is C++ itself, which has advanced by leaps and bounds in recent years, after a very long period of stifling stillness. New language features of C++11/14 such as lambdas, the support for concurrency, and many more are changing the way C++ "ticks" (in very good ways).

### The "vision" - what I'd like to see

What I think C++ needs now is a GUI library that is designed according to the central C++ tenet: full-featured with everything that a programmer needs to get going, and without any limits as to how far it can be extended; but also without any baggage that gets carried into an application that does not need or want it. And, perhaps most importantly: it should be so thoroughly cross-platform that it "just works" (this may seem a very lofty goal to aim for, but there is a possibility for a "fallback" option that just might make this a lot easier than it sounds - SDL2! I'll say something more about this further down in this paper.) 

Also, it should have the ability to be integrated with most any graphics library or framework, so that programmers won't have to look elsewhere because of an actually unrelated implementation choice. (In particular, it should be usable from within a game engine.)

Finally, and that might turn out to be the greatest challenge: the library should support near-perfect internationalization, just as C++ itself (almost) does. This means not only relying on Unicode, but also going the extra mile to support cultures that write right-to-left, upside-down, or bottom-up. (And I  believe that all these combinations actually do exist!)

### A few cornerstones of how to go about this

#### Rendering 

One of those prerequisites was defining an interface - as a pseudo-concept, not a binary one - that would encapsulate the rendering tasks, i.e. drawing graphical elements. I kept this very simple: all it has to be able to render are coloured boxes, images, and glyphs. (Extended concepts with additional functionality are a possibility, but the "base level" must be kept as simple as possible.) To date, I have implemented this "concept" for OpenGL 4.

At this time, a preliminary implementation of an OpenGL renderer exists and is being used both for development purposes and productively (though on a single project).

#### Pre-rasterizable fonts

I wrote a command-line utility, based on FreeType, that rasterizes a given font at a given size, and outputs the bitmaps in a simple binary format. This is in support of a planned feature that, to my knowledge, few if any GUI libraries have: the ability to do without a font rasterizer at runtime and instead use pre-rasterized glyphs. This could be extremely useful for applications such as OS installers, which should typically be as lightweight and free of dependencies as possible; but also of course for embedded devices with limited resources.

#### Cultural adaptability

I have also given some thought as to how to support internationalization, and come up with the idea of "cultural" directions as opposed to "geographical" directions for layouting. Cultural directions should always be preferred (where applicable), and mainly consist of a "culture-major" direction and a "culture-minor" direction, which together would describe how people read: in western culture, the "major" reading direction would be from top to bottom, whereas the "minor" reading direction would be from left to right. (I would welcome feedback on this issue - whether it seems at all sound, whether the terminology seems correct, and where to find pertinent information or existing studies on the subject.)

#### Platform/library adapters

One thing that follows pretty directly from the "vision" above is the consistent usage of "adapters": for the graphics library (if any!), for the audio (which I see as an optional way to improve the "feel" of the UI), and for keyboard, mouse, and touch input. All these adapters however should be at the source (or "concept") level, not binary. Moreover, all the adapter functions should be inlinable and as "thin" as possible, so that the compiler can optimize and hopefully completely eliminate any unnecessary code.

#### Separating look & feel from functionality

Another cornerstone is the separation between "view" and "model". It is probably a good idea to make that separation anyway, but in my proposal, the reason that makes this mandatory is - once more - "don't pay for what you don't need": the library should make it possible to implement a wide range of view implementations, whose capabilities go from the absolute minimum to fully themable with all conceivable bells and whistles. ("Out of the box", the library should probably offer both a minimal and an "intermediary-level" collection of views.)

Theming is the perfect example of a non-mandatory view feature. A minimal view collection would not have theming hooks of any kind - in fact, it would probably not support any kind of styling at all -, whereas the "intermediary" collection might provide some hooks by which to inject colors, decorative icons etc. by means other than coding (or via initialization code that could be made constexpr).

#### Full-featured, but potentially design-time layouting

Another detail that follows from the central tenet is the fact that layouting, while essential to my proposed library, does not necessarily have to be available at runtime. When dealing with a display of known size, and when there is no need for the user to change the layout (or the fonts or other style elements) at runtime, then positions and sizes can be determined at "design time" (i.e. within a GUI design utility program) and streamed to a binary file - which could even be "baked" into the target application as constant data.

<a name="adapt_to_mo"></a>
#### Adaptability to application's modus operandi

To make it possible for the UI library to be integrated into a wide range of application types, view implementations need to be flexible in the way they render themselves. For example, a game typically renders in a free-running "game loop" (sometimes limited by vsync or some other type of frame limiter), and in such a context, it would probably be acceptable to just re-draw the entire UI every time the game loop is traversed. 

On the other hand, a free-running loop is far from ideal in terms of power consumption, and thus out of the question for a mobile device. In such a case, it makes far more sense to try and limit redraws to only the parts that need updating.

There is also the question of when exactly redraws should happen. Aside from the game loop scenario, there are times when parts of the UI must be refreshed even when they haven't changed (such as when the window gets resized, though that might also trigger a reflow of the layout and thus require a full, or at least a bigger, redraw). However, the views' rendering "layer" (not the same as the Renderer concept mentioned above) should also support redrawing parts of the UI directly.

#### Supporting both "3D" and "2D" types of graphics hardware

Also, there are some optimizations that can be made when the hardware is exclusively "2D" oriented. In the history of Windows, there was a time when the term "graphics accelerator" did not mean 3D graphics, but the ability to shift portions of the screen, in other words when it made scrolling faster. Though it is unlikely that such graphics hardware comes back into fashion for PCs or even for smartphones, hardware with such capabilities could nowadays be created by programming ASICs, which would make it possible to attach standard LCD monitors to all kinds of embedded devices. More importantly though, this kind of so-called "blitting" operations can be done efficiently using the multimedia extensions of present-day Intel/AMD CPUs, making the support of "2D acceleration" a potentially very interesting feature for Linux OS installers that want to use the so-called "framebuffer" before the "real" graphics drivers are installed.

3D graphics-enabled hardware however can also benefit from optimizations, though of very different kinds. One possibility that comes to mind is the use of layers (typically, but not necessarily, implemented via off-screen buffers) that could be used for effects such as drag-and-drop.

The proposed GUI library should make it as easy as possible to optimally support whatever hardware capabilities are available.
