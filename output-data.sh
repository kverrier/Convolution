#!/bin/bash

N_TIMES=100
SAMPLE_RATE=44100

echo -e "IR_LEN\t1\t2\t3\t4\t5\t6"

for OUT_SIZE in 64 256 512
do

  for IMP_SIZE in 32768 65536
  do
    LATENCY=`echo "$OUT_SIZE / $SAMPLE_RATE * 1000" | bc -l`

    T1RATE=`./convolution -r $IMP_SIZE -n $N_TIMES -o $OUT_SIZE -t 1`
    instances=`echo "$LATENCY / ($T1RATE * $OUT_SIZE)" | bc -l`

    #echo "1, $T1RATE, 1, $instances"
    echo -n "$IMP_SIZE, "
    echo -n "$instances, "

    for THREADS in 2 3 4 5 6 7 8 9 10 11 12
    do
      rate=`./convolution -r $IMP_SIZE -n $N_TIMES -o $OUT_SIZE -t $THREADS`
      instances=`echo "$LATENCY / ($rate * $OUT_SIZE)" | bc -l`
      speed=`echo "$T1RATE / $rate" | bc -l`
      #echo -e "$THREADS,\t $rate,\t $speed,\t $instances"
      echo -n "$instances, "
    done
    echo ""
  done
    echo "-----------"
done
