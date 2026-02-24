#!/bin/sh
BINARY_PATH="$1"
DIST_DIR="$2"
ZIP_NAME="$3"
FORCE_TARGET="$4"
FORCE_DEST="$5"
BIN_NAME=$(basename "$BINARY_PATH")

arm-linux-gnueabihf-strip --strip-unneeded "$BINARY_PATH"
cp "$BINARY_PATH" "$DIST_DIR/$BIN_NAME"
chmod +x "$DIST_DIR/$BIN_NAME"

find "$DIST_DIR" -name "*.sh" -exec chmod +x {} + 2>/dev/null || true

cd "$DIST_DIR" && zip -rj "../$ZIP_NAME" . && cd -

SSH_OPTS="-o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -o ConnectTimeout=5"
echo "Deploying to $FORCE_TARGET..."
ssh $SSH_OPTS root@$FORCE_TARGET "mkdir -p $FORCE_DEST && killall $BIN_NAME 2>/dev/null || true"
scp -r $SSH_OPTS "$DIST_DIR/." root@$FORCE_TARGET:"$FORCE_DEST"
ssh $SSH_OPTS root@$FORCE_TARGET "chmod +x $FORCE_DEST/$BIN_NAME"
