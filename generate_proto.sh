#!/bin/bash

# Generate nanopb files from proto definition
# This script should be run when the .proto file changes

PROTO_DIR="proto"
OUT_DIR="lib/RoverProto/src"

# Create output directory if it doesn't exist
mkdir -p $OUT_DIR

# Generate nanopb files
protoc --nanopb_out=$OUT_DIR $PROTO_DIR/rover_service.proto

echo "Generated protobuf files in $OUT_DIR"