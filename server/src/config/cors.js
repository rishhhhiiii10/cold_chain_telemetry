const allowedOrigins = (process.env.CLIENT_URL || "http://localhost:5173,http://localhost:4173")
  .split(",")
  .map((origin) => origin.trim());

module.exports = {
  origin(origin, callback) {
    if (!origin || allowedOrigins.includes(origin)) {
      callback(null, true);
    } else {
      callback(new Error("Not allowed by CORS"));
    }
  },
  credentials: true,
};
