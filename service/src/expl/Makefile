all: payload.bin.sla_timeout payload.bin.accepted

payload.bin.sla_timeout: ./pwnload.rb
	../../src/mrbc -Eblah ./pwnload.rb
	python3 ./fakesign.py ./pwnload.mrb
	python3 ./pack_pwnload.py ./pwnload.mrb ./sig ./payload.bin.sla_timeout

payload.bin.accepted: ../verify/payload.mrb
	python3 ./fakesign.py $<
	python3 ./pack_pwnload.py $< ./sig $@
