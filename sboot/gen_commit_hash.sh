#!/bin/sh
#
# Handy script to generates mboot hash and corresponding repository name.
#

echo "----------------------------------------------------------------------------"
repo forall -c '{ git rev-parse HEAD | tr '"'"'\n'"'"' '"'"' '"'"' ; echo "@ $REPO_PROJECT"; } | cat'
echo "----------------------------------------------------------------------------"
