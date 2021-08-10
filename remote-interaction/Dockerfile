FROM ubuntu:20.04
ENV TERM linux
run sed -i 's|http://|http://us.|g' /etc/apt/sources.list

run apt-get -qq update && apt-get install -qq --no-install-recommends python3 python3-pip python-setuptools
run apt-get install -qq --no-install-recommends build-essential python-dev

COPY requirements.txt /requirements.txt
RUN pip install -r requirements.txt

COPY check1.py /check1.py
COPY exploit1.py /exploit1.py
COPY exploit2.py /exploit2.py
COPY wsprox.py /wsprox.py
