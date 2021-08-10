#!/bin/bash
#websocketd --port=8080 ruby ./spawn.rb
socat TCP-LISTEN:3000,fork,reuseaddr EXEC:'./spawn.rb'
