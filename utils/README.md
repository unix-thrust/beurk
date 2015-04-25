# Developper Utilities
The `./utils/` directory provides a collection of useful scripts for
BEURK developers.


## Guidelines
If you plan to add a script into **utils**, please follow the following
conventions:

#### 1 - Script Name
  * A lowercase and dash separated name, with the proper language extension.
  * The title himself should be enough to understand what it does.
  > **E.G:** `deploy-git-hooks.sh`

#### 2 - Script Content
  * Write it with the language you want. no matters.
  * Add a block of comments at the start of the script which explains
    what the script does exactly.
  * Consider commenting (or avoiding) tricky parts of your code for
    easing maintenability.
  * If you write it in pascal, you just deserve to die ...

#### 3 - Integration
  * If your script is useful (and i hope for you that he is),
    think to talk about it on related project wiki pages.
  * Add an entry with a short description in the **Scripts Collection**
    section below.


## Scripts Collection

### `deploy-git-hooks.sh`
Deploy git presubmit hooks in order to check if commit messages
follow our **Commit Message Conventions**.
> NOTE: This script still needs to be written !
