#!/bin/bash -e
cd "$(dirname "$0")"
case "$1" in
  --apply) source .missing_timestamps ;;
  --store)
    STAT='stat -c %Y'
    $STAT .git > /dev/null 2>&1 || STAT='stat -f %m'  # OSX
    _date() { date +%Y%m%d%H%M.%S -d@$1; }
    _date 12345 > /dev/null 2>&1 || _date() { date -r$1 +%Y%m%d%H%M.%S; }  # OSX
    while read -d $'\0' FILE; do
      [[ "$FILE" == . ]] && continue
      TIME=$(_date $($STAT "$FILE"))
      echo "touch -c -t ${TIME} \"${FILE}\""
    done < <(find . -name '.git' -prune -o -name '*' -print0) > .missing_timestamps
  ;;
  *) exit 42 ;;
esac
