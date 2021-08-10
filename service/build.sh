#!/bin/bash -ex
IMAGE=barb-metal-build
CONTAINER=barb-metal-build-container

pushd src
docker build -t $IMAGE .
popd

rm -f payload.bin service
docker run --rm --name=$CONTAINER -d $IMAGE /bin/bash -c "sleep 1000"
docker cp $CONTAINER:/build/src/verify/payload.bin payload.bin
docker cp $CONTAINER:/build/src/verify/verify service.dbg
docker cp $CONTAINER:/build/src/expl/payload.bin.accepted ../patches/payload.bin.accepted
docker cp $CONTAINER:/build/src/expl/payload.bin.sla_timeout ../patches/payload.bin.sla_timeout
cp service.dbg service
cp service.dbg ../local-tester/service.dbg
strip service
docker kill $CONTAINER
