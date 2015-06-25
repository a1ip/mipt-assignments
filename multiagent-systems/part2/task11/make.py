#!/usr/bin/python

import json
import sys

assert len(sys.argv) == 4

services = int(sys.argv[1])
prepare_timeout = float(sys.argv[2])
put_timeout = float(sys.argv[3])

yml = """
rabbitmq:
  image: rabbitmq

putter:
  build: ./putter
  volumes:
   - ./putter:/code
  links:
   - rabbitmq
"""

for i in xrange(services):
  yml += """
d{0}:
  build: ./dict
  volumes:
   - ./dict:/code
  links:
   - rabbitmq
  command: ./run.sh dict:DictService{0}
""".format(i)

with open('docker-compose.yml', 'w') as f:
  f.write(yml)

params = {'timeout': prepare_timeout, 'services': services}
with open('dict/params.json', 'w') as f:
  f.write(json.dumps(params))

params = {'timeout': put_timeout}
with open('putter/params.json', 'w') as f:
  f.write(json.dumps(params))
