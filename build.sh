#!/bin/bash

###############################################################################
# Title: Bash Getopts - 让你的脚本支持命令行参数
# Web: https://linux.cn/article-3204-1.html
###############################################################################

VERBOSE=false
COVERAGE=OFF
SANITIZE=OFF
DEBUG=OFF

#Set Script Name variable
SCRIPT=`basename ${BASH_SOURCE[0]}`

#Set fonts for Help
NORM=`tput sgr0`
BOLD=`tput bold`
REV=`tput smso`

#Help function
function HELP {
  echo -e "${REV}Basic usage:${NORM} ${BOLD}$SCRIPT${NORM}"\\n
  echo "${REV}-s${NORM} --Sets option ${BOLD}SANITIZE${NORM} ON."
  echo "${REV}-c${NORM} --Sets option ${BOLD}OVERAGE${NORM} ON."
  echo "${REV}-v${NORM} --Sets option ${BOLD}VEROSE${NORM} ON."
  echo "${REV}-d${NORM} --Sets option ${BOLD}DEBUG${NORM} ON."
  echo -e "${REV}-h${NORM} --Displays this help message. No further functions are performed."\\n
  echo -e "Example: ${BOLD}$SCRIPT -scv${NORM}"\\n
  exit 1
}

while getopts vcshd opt; do
  case ${opt} in
    v)
       VERBOSE=true
       echo "-v VERBOSE used." 
       ;;
    c)
       COVERAGE=ON
       echo "-c COVERAGE used."
       ;;
    s)
       SANITIZE=ON
       echo "-s SANITIZE used"
       ;;
    d)
       DEBUG=ON
       echo "-d DEBUG used"
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
if [ ${SANITIZE} = ON ]; then
    BUILDDIR=_build_sanitize
    COVERAGE=OFF
elif [ ${COVERAGE} = ON ]; then
    BUILDDIR=_build_coverage
fi

mkdir ${BUILDDIR} > /dev/null
pushd ${BUILDDIR} > /dev/null

[[ ${DEBUG} = ON ]] && BUILD_TYPE=Debug || BUILD_TYPE=Release  #https://codeday.me/bug/20170318/7493.html
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -Dsanitize=${SANITIZE} -Dcoverage=${COVERAGE} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..

if [ ${VERBOSE} = true ]; then
    make VERBOSE=1
else
    make
fi

if [ ${SANITIZE} = ON ]; then
  ./MainTest
elif [ ${COVERAGE} = ON ]; then
  make test
  ctest
  make Test_coverage
fi

popd > /dev/null
