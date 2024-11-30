# Snake-84
For code documentation, take a look at the header in src/

## Overview
Just the snake game, my take on it at least
- Up/Down/Right/Left buttons for movement, also 5/2/6/4 on the numpad
- Enter to pause, mostly done for debugging reasons, might remove
Of course if a feature is removed and you were a fan, feel free to
fork the repo and implement it yourself (:
- 2nd to quit

### TODO
- Optimize overall for size
- Remove the hard cap on snake length
- Save high score
- Menu with settings
- Documentation

## Installation
This was written for the TI-84 CE Plus, should work whether Python or not

To install the pre-built `SNAKE.8xp` in `bin/` you will need:
- TI Connect CE
    - connect the calculator, and then you can drag and drop the file
- `clib.8xg` [download here](https://github.com/CE-Programming/libraries/releases/tag/v12.1)
    - NOTE: On MacOS the same error listed here for ChromeOS might apply -- it did for me,
    so I would recommend getting the separate version and loading the files individually
- [Artifice](https://yvantt.github.io/arTIfiCE/)
    - Because of 'safety measures' put into TI calculators with newer OS versions,
    this allows you to get around it and launch the game, otherwise you'll
    encounter an error.

## Building
If you want to build this locally, you'll need the [CE Toolchain](https://github.com/CE-Programming/toolchain)
The instructions are pretty well documented [here](https://ce-programming.github.io/toolchain/index.html)

## Common Errors
- 'Invalid Token' make sure you're using Artifice
- 'Library Version: Libload' use the separate clibs files zip

