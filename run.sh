#!/usr/bin/bash
cd build
cmake .. && make && ./RayTracingInOneWeekend > image.ppm
cd ..
