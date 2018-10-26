#!/bin/bash

###############################################################################
# Title: Bash Getopts - 让你的脚本支持命令行参数
# Web: https://linux.cn/article-3204-1.html
###############################################################################

VERBOSE=false
COVERAGE=OFF

#Set Script Name variable
SCRIPT=`basename ${BASH_SOURCE[0]}`

#Set fonts for Help
NORM=`tput sgr0`
BOLD=`tput bold`
REV=`tput smso`

#Help function
function HELP {
  echo -e "${REV}Basic usage:${NORM} ${BOLD}$SCRIPT${NORM}"\\n
  echo "${REV}-v${NORM} --Sets option ${BOLD}VEROSE${NORM} ON."
  echo "${REV}-c${NORM} --Sets option ${BOLD}CODE COVERAGE${NORM} ON."
  echo -e "${REV}-h${NORM} --Displays this help message. No further functions are performed."\\n
  echo -e "Example: ${BOLD}$SCRIPT -vc${NORM}"\\n
  exit 1
}

while getopts vhc opt; do
  case ${opt} in
    v)
       VERBOSE=true
       echo "-v used" 
       ;;
    c)
       COVERAGE=ON
       echo "-c used"
       ;;
    h)
       HELP
       ;;
    \?)
       HELP
       ;;
    esac
done
shift "$((OPTIND-1))"

BUILDDIR=_build
if [ ${COVERAGE} = ON ]; then
    BUILDDIR=_build_coverage
fi

mkdir ${BUILDDIR} > /dev/null
pushd ${BUILDDIR} > /dev/null
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -Dcoverage=${COVERAGE} ..

if [ ${VERBOSE} = true ]; then
    make VERBOSE=1
else
    make
fi

if [ ${COVERAGE} = ON ]; then
    make test
    ctest
    make Test_coverage
fi

popd > /dev/null
