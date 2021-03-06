#!/bin/bash

sudo apt install -y golang-go
sudo apt install -y go-dep

GOPATH=~/go ; export GOPATH
git clone https://github.com/improbable-eng/grpc-web.git $GOPATH/src/github.com/improbable-eng/grpc-web
pushd $GOPATH/src/github.com/improbable-eng/grpc-web
dep ensure # after installing dep
go install ./go/grpcwebproxy # installs into $GOPATH/bin/grpcwebproxy
popd

