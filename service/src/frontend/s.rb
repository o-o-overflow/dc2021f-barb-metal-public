require 'sinatra'
require 'sinatra-websocket'
require 'socket'
require 'base64'

set :server, 'thin'
set :bind, '0.0.0.0'
set :port, 7829
set :sockets, []
set :environment, :production

puts "STARTING"

system("/usr/sbin/xinetd -filelog /dev/stderr -dontfork -f /service.conf &")
prompt = "barbOS> "
get '/' do
  if !request.websocket?
    IO.read("./barb.html")
  else
    request.websocket do |ws|
      s = TCPSocket.new 'localhost', 7828
      t = Thread.new do
        str = ""
        loop do
          x = s.read(1)
          str << x
          if x == "\n" or str.include? prompt
            ws.send(Base64.encode64(str))
            str = ""
          end
        end
      end
      ws.onopen do
        settings.sockets << ws
      end
      ws.onmessage do |msg|
        #EM.next_tick { settings.sockets.each{|s| s.send(msg) } }
        begin
          s.write(Base64.decode64(msg))
        rescue
        end
        #puts("received #{Base64.decode64(msg)}")
      end
      ws.onclose do
        warn("websocket closed")
        settings.sockets.delete(ws)
        s.close
        Thread.kill(t)
      end
    end
  end
end
