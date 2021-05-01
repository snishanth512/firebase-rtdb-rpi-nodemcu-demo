#! /bin/bash

stress-ng --timeout 30s --cpu 4 -vm 1 --vm-bytes 75% --cpu-method fft
