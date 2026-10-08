#!/bin/bash

echo "================================="
echo "   MULTI-PROCESS SIMULATOR"
echo "        BENCHMARK TEST"
echo "================================="

echo "Starting benchmark..."

START=$(date +%s%N)

./launcher <<EOF2
ADD 10 20
EXIT
EOF2

END=$(date +%s%N)

ELAPSED=$(( (END - START) / 1000000 ))

echo ""
echo "Benchmark completed."
echo "Execution time: ${ELAPSED} ms"
echo "================================="
