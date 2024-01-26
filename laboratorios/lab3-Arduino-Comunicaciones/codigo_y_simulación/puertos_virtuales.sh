#!/bin/sh
socat PTY, link=/tmp/tty50, raw, echo=0 PTY, link=/tmp/tty51, raw, echo=0
