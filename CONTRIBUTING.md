# Contributing
This document seeks to describe how and what to contribute.

# How to contribute
If you are a new contributor, please check out the DEVELOPERS.md document if
you intend to contribute code.  This contains important details like project
structure, compiling and testing mycroft, coding style, and much more.

# What to contribute

## New features
mycroft core is focused around interacting with files and providing a platform
for analysing them.  any new features that assist in developing the base
framework to achieve those goals are welcome.  Any more use-specific 

## Templates
out of the box, we'd like to cover as many file formats as practically and
legally possible.  if you'd like to contribute a template for a filetype,
feel free to pull request it.  If your template involves processing or analysis
code, submit it instead as a plugin.

## Types
if you'd like to support an interpreter type, feel free to do so.  type
submissions should be akin to primitive types in programming languages.  if your type
has a lot of processing or external dependencies, submit it as a plugin.

## Plugins
Mycroft features a plugin system with which you can use to extend its base
functionality.  All non-hex-editing and template stuff should be implemented
and submitted as a plugin versus an extention on the core or any of the
clients.

## Bugs and tests
If you come up with a fun way to be a dick to mycroft components (or want to),
please feel free to submit tests and bugs.  A super powerful way to make sure
your software works is to try as many ways as possible to break it, and tests
will help us automate the process of checking it!  See the DEVELOPERS.md
document for more information on writing tests.  If you just want to report a
bug you found without a test, submit it as an issue.

## Corrections and optimizations
Mycroft isn't perfect.  If you see any code you think is inefficient or could
be improved in some way, feel free to contribute a correction.  These may
be submitted as issues or as pull requests (if you have a solution).  The
latter is MUCH more preferable for obvious reasons.  Please be prepared to
upporting documentation to justify your proposed change if it's not trivial
or potentially controversial.

Additionally, we also welcome corrections relating to grammar, spelling and
other mistakes in usage of the English language.  Subjective corrections like
use of "inappropriate" or "exclusive" speech will be considered on a case by
case basis, but chances are if it made it into the repo it's going to stay.
You're welcome to raise it as an issue if you feel passionately about it,
though!  We might miss simple things like use of gender-specific pronouns,
which can be changed easily and with no fuss.  Also, don't bother trying to
push code-of-conduct documents, I don't see it necessary to adopt one (and
more importantly I hate them).

## Documentation
If you'd like to contribute some of documentation (how-tos, examples, etc),
these are VERY welcome.  Such literature greatly increases the ease in which
new users and developers can get started using/working on mycroft, and we would
like to add as much as possible to the project.  Our goal regarding
documentation is that a developer or user should be able to clone this repo and
be able to develop or use mycroft without any external resources, so anything
you think might be useful to others should be submitted!

## Translations
If you'd like to contribute a full or partial translation (preferably the
former) of mycroft and/or its documentation to a new language, please check out
the DEVELOPERS.md document for help on getting started.  Feel free to submit
your translation as a pull request.  Please note that adoption of translations
might be delayed as we will attempt to verify the accuracy of your work prior
to its inclusion in mycroft's code base.

