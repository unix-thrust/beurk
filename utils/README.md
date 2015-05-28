# Developper Utilities
The `./utils/` directory provides a collection of useful scripts for
BEURK developers.

  * If you think your script is useful (it probably won't be),
    please submit an issue beforehand (We automatically reject PRs
    that involves modifying our scripts for obvious security reasons).

## Guidelines
If you plan to add a script into **utils**, please follow the following
conventions:

#### 1 - Script Name
  * A lowercase and dash separated name, with the proper language extension.
  * The title itself should be enough to understand what it does.
  > **E.G.:** `deploy_git_hooks.sh`

#### 2 - Script Content
  * Write it in the language of your choice, it doesn't matter. Aside from
    Pascal, we're not THAT horrible.
  * Add a block of comments at the start of the script explaining
    what the script does in detail.
  * Consider commenting tricky parts of your code (or don't do that
    at all) to ease maintenance.

#### 3 - Integration
  * Add an entry with a short description in the **Scripts Collection**
    section below.
  * Edit the related wiki entry if any page can be related to the
    script.

## Scripts Collection

### `deploy_git_hooks.sh`
Deploy git presubmit hooks in order to check automatically if
commit messages are following our **Commit Message Guidelines**.

### `run-tests.sh`
Run a test script or all tests contained in a given directory,
in a recursive manner.
