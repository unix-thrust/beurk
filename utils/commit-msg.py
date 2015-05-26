#!/usr/bin/env python2

"""
Git commit hook

Check commit message according to BEURK commit guidelines
"""

import sys, os, re
from subprocess import call


valid_commit_types = ['feat', 'fix', 'docs', 'style', 'refactor',
        'perf', 'test', 'chore', ]
valid_commit_scopes = ['core', 'builder', 'client', ]
help_address = 'https://github.com/unix-thrust/beurk/wiki/Commit-Guidelines'

is_piped = True if len(sys.argv) == 1 else False
editor = os.environ.get('EDITOR', 'vim')

def bad_commit(errmsg, line=""):
    if line:
        sys.stderr.write("\nThe following line does not follow our "
                "guidelines:\n%s\n" % line)
    sys.stderr.write("\n%s\n" % errmsg)
    sys.stderr.write("\nPlease refer to our commit guidelines:\n - %s\n"
            % help_address)
    if is_piped:
        sys.exit(1)
    raise SyntaxError(errmsg)

while True:
    commit = sys.stdin if is_piped else open(sys.argv[1], 'r+a')
    try:
        lines = commit.read().splitlines()
        # abracadabra: remove all comments from the list of lines ;)
        lines = [l for l in lines if not l.startswith("#")]

        if len(lines) == 0:
            bad_commit(commit, "Empty commit message")

        # first line
        line = lines[0]

        # ignore any Merge
        if line.startswith("Merge"):
            sys.exit(0)

        if len(line) > 50:
            bad_commit("First commit message line (header) "
                    "is exceeding the 50 chars limit", line)

        m = re.search('^(.*)\((.*)\): (.*)$', line)

        if not m or len(m.groups()) != 3:
            bad_commit("First commit message line (header) does not "
                    "follow format: type(scope): message", line)

        commit_type, commit_scope, commit_message = m.groups()

        if commit_type not in valid_commit_types:
            bad_commit("Commit type not in valid ones: %s"
                    % ", ".join(valid_commit_types), line)

        if commit_scope not in valid_commit_scopes \
                and commit_type not in ["docs", "chore"]:
            bad_commit("Commit scope not in valid ones: %s"
                    % ", ".join(valid_commit_scopes), line)

        if commit_message[0].isupper():
            bad_commit("Commit subject first char not lowercase", line)

        if commit_message.endswith('.'):
            bad_commit("Commit subject last char (a dot) "
                    "is not allowed", line)

        verb = commit_message.split()[0]
        if verb.endswith("ing") or verb.endswith("ed"):
            bad_commit("Commit subject must use imperative, present tense:\n"
                    "# \"change\", not \"changed\" nor \"changing\"", line)

        if line != line.strip():
            bad_commit("First commit message line (header) "
                    "contains leading or ending spaces", line)

        if len(lines) > 1 and lines[1]:
            bad_commit("Second commit message line must be empty")

        if len(lines) > 2 and not lines[2].strip():
            bad_commit("Third commit message line (body) "
                    "can't be empty", lines[2])

        for l in lines:
            if len(l) > 72:
                bad_commit("This line is exceeding the 72 chars limit", l)

    # We catch that an error has happened and react accordingly
    except SyntaxError as err:
        if raw_input("Do you want to edit it? (Your commit will "
                "be rejected otherwise) [Y/n] ").lower() == 'y':
            commit.write("#\n# %s\n#" % err)
            commit.write("\n# - Please refer to our commit guidelines:\n"
                    "# %s\n" % help_address)
            sys.stderr.write('\n')
            commit.close()
            call('%s %s' % (editor, sys.argv[1]), shell=True)
            continue
        else:
            sys.exit(1)
    break
sys.exit(0)
