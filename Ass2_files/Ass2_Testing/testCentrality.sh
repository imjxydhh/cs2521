RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'
n="6"
make || exit
mkdir -p output
if [ $# -eq 1 ]; then
  i=$1
  for t in 'c' 'b' 'bn'; do
    ./testCentralityMeasures graphs/$i.in $t > output/$i$t.out
    r=`diff graphMeasurements/$i$t.out output/$i$t.out`
    if [[ "$r" == "" ]]; then
      echo -e "====== ${GREEN}[$i][$t] Output Matches${NC} ======"
    else
      echo -e "=========== ${RED}[$i][$t] Output Mismatch${NC} ============"
      diff graphMeasurements/$i$t.out output/$i$t.out
    fi
    rm output/$i$t.out
  done
  exit
elif [ $# -eq 2 ]; then
  i=$1
  t=$2
  ./testCentralityMeasures graphs/$i.in $t > output/$i$t.out
  r=`diff graphMeasurements/$i$t.out output/$i$t.out`
  if [[ "$r" == "" ]]; then
    echo -e "====== ${GREEN}[$i][$t] Output Matches${NC} ======"
  else
    echo -e "=========== ${RED}[$i][$t] Output Mismatch${NC} ============"
    diff graphMeasurements/$i$t.out output/$i$t.out
  fi
  rm output/$i$t.out
  exit
fi
for i in `seq 1 $n`; do
  for t in 'c' 'b' 'bn'; do
    ./testCentralityMeasures graphs/$i.in $t > output/$i$t.out
    r=`diff graphMeasurements/$i$t.out output/$i$t.out`
    if [[ "$r" == "" ]]; then
      echo -e "====== ${GREEN}[$i][$t] Output Matches${NC} ======"
    else
      echo -e "=========== ${RED}[$i][$t] Output Mismatch${NC} ============"
      diff graphMeasurements/$i$t.out output/$i$t.out
    fi
    rm output/$i$t.out
  done
done
