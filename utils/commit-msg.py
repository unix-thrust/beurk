#!/usr/bin/env python2

"""
Git commit hook

Check commit message according to BEURK commit guidelines
"""

import sys
import re

valid_commit_types = ['feat', 'fix', 'docs', 'style', 'refactor',
        'perf', 'test', 'chore', ]
valid_commit_scopes = ['core', 'builder', 'client', ]
commit_file = sys.argv[1]
help_address = 'https://github.com/unix-thrust/beurk/wiki/Commits-Guidelines'

with open(commit_file) as commit:
    lines = commit.read().splitlines()
    # abracadabra: remove all comments from the list of lines ;)
    lines = [l for l in lines if not l.lstrip().startswith("#")]
    if len(lines) == 0:
        sys.stderr.write("\nEmpty commit message\n")
        sys.stderr.write("\n - Please refer to commit guide: %s\n\n"
                % help_address)
        sys.exit(1)

    # first line
    line = lines[0]
    if len(line) > 50:
        sys.stderr.write("\nFirst commit message line (header) "
                "is exceeding the 50 chars limit\n")
        sys.stderr.write("\n - Please refer to commit guide:"
                "%s\n\n" % help_address)
        sys.exit(1)

    m = re.search('^(.*)\((.*)\): (.*)$', line)

    if not m or len(m.groups()) != 3:
        sys.stderr.write("\nFirst commit message line (header) "
                "does not follow format: type(scope): message\n")
        sys.stderr.write("\n - Please refer to commit guide: "
                "%s\n\n" % help_address)
        sys.exit(1)
    commit_type, commit_scope, commit_message = m.groups()
    if commit_type not in valid_commit_types:
        sys.stderr.write("\nCommit type not in valid ones: %s\n"
                % ", ".join(valid_commit_types))
        sys.stderr.write("\n - Please refer to commit guide: %s\n\n"
                % help_address)
        sys.exit(1)
    if commit_scope not in valid_commit_scopes and commit_type != "docs" and commit_type != "chore":
        sys.stderr.write("\nCommit scope not in valid ones: %s\n"
                % ", ".join(valid_commit_scopes))
        sys.stderr.write("\n - Please refer to commit guide: %s\n\n"
                % help_address)
        sys.exit(1)
    if commit_message[0].isupper():
        sys.stderr.write("\nCommit subject first char not lowercase\n")
        sys.stderr.write("\n - Please refer to commit guide: %s\n\n"
                % help_address)
        sys.exit(1)
    if commit_message[-1] == '.':
        sys.stderr.write("\nCommit subject last char (a dot) is not allowed\n")
        sys.stderr.write("\n - Please refer to commit guide: %s\n\n"
                % help_address)
        sys.exit(1)

    if line != line.strip():
        sys.stderr.write("\nFirst commit message line (header) "
                "contains leading or ending spaces\n")
        sys.stderr.write("\n - Please refer to commit guide: %s\n\n"
                % help_address)
        sys.exit(1)

    if len(lines) > 1:
        sys.stderr.write("\nSecond commit message line must be empty\n")
        sys.stderr.write("\n - Please refer to commit guide: %s\n\n"
                % help_address)
        sys.exit(1)

    if len(lines) > 2 and not lines[2].strip():
        sys.stderr.write("\nThird commit message line (body) "
                "must not be empty\n")
        sys.stderr.write("\n - Please refer to commit guide: %s\n\n"
                % help_address)
        sys.exit(1)

    for l in lines:
        if len(l) > 72:
            sys.stderr.write("\nFollowing line is exceeding the "
                    "72 chars limit:\n")
            sys.stderr.write(l)
            sys.stderr.write("\n - Please refer to commit guide: %s\n\n"
                    % help_address)
            sys.exit(1)

sys.exit(0)
