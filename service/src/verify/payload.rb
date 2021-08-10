def putsDBG str
  puts "DEBUG: #{str}"
end

def split s, sep
  r = []
  m = ""
  len = s.size
  i = 0
  while i < len do
    if s[i].ord == sep.ord
      if m.size != 0
        r << m
        m = ""
      end
    else
      m << s[i]
    end
    i = i + 1
  end
  r << m
  return r
end

def gets()
  s = ""
  while true
    m = @uart.read(1)
    if m.ord == 13 or m.ord == 10
      break
    end
    s << m
  end
  return s
end


def info
  putsDBG "barbOS v0.0 : num_devices #{@devices.count} : last used device #{@lastused.class}"
end

@timeofday = {"day": 0, "night":1}
@dayofweek = {"sunday": 0, "monday":1, "tuesday":2, "wednesday":3, "thursday":4, "friday": 5, "saturday": 6}

@devices = []
@b64 = Base64.new()
@uart = UART.new()
@alarm = Alarm.new
@therm = Thermostat.new()
@speaker = Smartspeaker.new()

@devices << @therm
@devices << @alarm
@devices << @speaker

@lastused = @speaker


def set_therm s
  cmd = s.split(" ")
  time = cmd[2]
  if time.to_i != 0
    time = time.to_i
  end
  date = cmd[3]
  if date.to_i != 0
    date = date.to_i
  end
  temp = cmd[4].to_i
  if (time.class == String)
    time = @timeofday[time.to_sym]
    if time.nil?
      putsDBG "INVALID TIME"
      return
    end
  end
  if (date.class == String)
    date = @dayofweek[date.to_sym]
    if date.nil?
      putsDBG "INVALID DAY #{date}"
      return
    end
  end
  if (time != 0 and time != 1)
    putsDBG "INVALID TIME #{time}"
    return
  end
  if (date > @dayofweek.size)
    putsDBG "INVALID date #{date}"
    return
  end
  @therm.write(time, date, temp)
  return temp
end

def get_therm s
  cmd = s.split(" ")
  time = cmd[2]
  if time.to_i != 0
    time = time.to_i
  end
  date = cmd[3]
  if date.to_i != 0
    date = date.to_i
    if date > 0xFFFFFFFF
      date = 0
    end
  end
  if (time.class == String)
    time = @timeofday[time.to_sym]
    if time.nil?
      putsDBG "INVALID TIME"
      return
    end
  end
  if (date.class == String)
    date = @dayofweek[date.to_sym]
    if date.nil?
      putsDBG "INVALID DAY"
      return
    end
  end
  if (time < 0 or time > 1)
    putsDBG "INVALID TIME"
    return
  end
  if (date < 0 or time > 1) # bug to read anywhere in the future, can't read flag this way, but can leak memory for other use
    putsDBG "INVALID date"
    return
  end
  return @therm.read(time, date)
end

def do_therm line
  @lastused = @therm
  cmd = line.split(" ")
  if cmd[1] == "read"
    temp = get_therm(line)
    puts "#{cmd[2]} #{temp}"
  elsif cmd[1] == "set"
    temp = set_therm(line)
    putsDBG "set thermostat to #{temp}"
  end
end

@alarm_test = "beepBEEP"
def do_alarm line
  cmd = line.split(" ")
  if cmd[1] == "armed?"
    if @alarm.armed?
      puts "armed"
    else
      puts "not armed"
    end
  elsif cmd[1] == "arm"
    @alarm.arm
  elsif cmd[1] == "info"
    puts @alarm.info
  elsif cmd[1] == "settest"
    c2 = split line, " "
    s = c2[-1]
    len = s.size
    i = 0
    while i < len
      @alarm_test[i] = s[i]
      i = i + 1
    end
  elsif cmd[1] == "test"
    count = cmd[-1].to_i
    x = @alarm_test * count
    puts x
  elsif cmd[1] == "disarm"
    if (@alarm.disarm(cmd[2].to_i, cmd[3].to_i, cmd[4].to_i, cmd[5].to_i))
      puts "alarm disarmed"
    else
      puts "alarm disarm failed"
    end
  end
end

#do_alarm "ALARM armed?"
#do_alarm "ALARM arm"
#do_alarm "ALARM armed?"
#do_alarm "ALARM disarm 1 2 3 4"
#do_alarm "ALARM disarm 1 3 3 7"
#do_alarm "ALARM info"

def do_speaker line
  @lastused = @speaker
  cmd = split(line, " ")
  #cmd = line.split(" ")
  if cmd[1] == "queue"
    n = @speaker.add(cmd[2])
    puts "playing #{cmd[2]} next, there are #{n} in the queue"
  elsif cmd[1] == "play"
    puts "now playing #{@speaker.play}"
  elsif cmd[1] == "review"
    r = cmd[2].to_i
    s = cmd[3]
    puts "gave review of #{@speaker.vote(s, r)}"
  elsif cmd[1] == "popular"
    @speaker.queue_popular!
    puts "playing most popular song next"
  end
end

#do_speaker "SPEAKER queue song1"
#do_speaker "SPEAKER queue song2"
#do_speaker "SPEAKER queue song3"
#do_speaker "SPEAKER review 10 song2"
#do_speaker "SPEAKER popular"
#do_speaker "SPEAKER play"
#do_speaker "SPEAKER play"
#do_speaker "SPEAKER play"

puts Base64.decode64("d2VsY29tZSB0byBiYXJiT1MuLi4=")

# exploit
#puts @alarm.info
#set_therm("THERM set day -248 128")
#set_therm("THERM set night -248 18")
#set_therm("THERM set day -247  20")
#set_therm("THERM set night -247 0")
#puts "pwned?"
#puts @alarm.info

while true
  print "barbOS> "
  b_s = gets()
  #s = Base64.decode64(b_s)
  s = b_s # Plaintext for now
  cmd = s.split(" ")[0]
  case cmd
  when "THERM"
    do_therm(s)
  when "INFO"
    info
  when "ALARM"
    do_alarm(s)
  when "SPEAKER"
    do_speaker(s)
  when "HELP"
    puts "try reversing it..."
  when "help"
    puts "try reversing it..."
  else
    puts "invalid"
  end
end
