let io;

const cors = require("./cors");

function initializeSocket(server) {
  const { Server } = require("socket.io");

  io = new Server(server, {
    cors: {
      ...cors,
      methods: ["GET", "POST"],
    },
  });

  io.on("connection", (socket) => {
    console.log(`Socket connected: ${socket.id}`);

    socket.on("disconnect", (reason) => {
      console.log(`Socket disconnected: ${socket.id} (${reason})`);
    });
  });

  return io;
}

function getIO() {
  if (!io) {
    throw new Error("Socket.IO has not been initialized");
  }

  return io;
}

module.exports = {
  initializeSocket,
  getIO,
};
