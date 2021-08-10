#!/usr/bin/env ruby
require 'pty'
require 'expect'
require 'io/console'
STDOUT.sync = true
flag = File.read("./flag")
payload = File.binread("./payload.bin")

PTY.spawn('qemu-system-i386 -monitor /dev/null -serial stdio -display none -kernel verify -s') do |stdout, stdin, pid|
  sleep 0.1
  Thread.new {
    while true do
      begin
        print stdout.getch()
      rescue
      end
    end
  }
  stdin.write flag
  payload.each_byte do |i|
    stdin.write i.chr
    sleep 0.0001
  end
  while true do
    begin
    s = STDIN.gets.chomp
    stdin.write(s + "\r")
    rescue
      # do nothing
    end
  end
end

