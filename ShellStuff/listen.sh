#!/bin/bash
for i in {1..500}
do
   curl 192.168.1.23 >> log
done