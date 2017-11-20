# Change the current order of the awk print is to convert FreeRTOS style to PROS 
# style. Change from `$1, $2` to `$2, $1` to go the other way.
cat src/rtos/refactor.tsv | dos2unix | awk '/^\s*$/ {next;} {if(NR>1) { printf "s/\\\\<%s\\\\>/%s/g\n", $1, $2 } }' | xargs -I '{}' zsh -c 'echo "{}"; sed -i "{}" **/*.(h|c|md|s|S|cpp|c++|cc)'
