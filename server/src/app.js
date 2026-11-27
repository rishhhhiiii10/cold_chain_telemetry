const express = require("express");
const cors = require("cors");
const helmet = require("helmet");

const app = express();

// Middleware
app.use(helmet());

app.use(cors(require("./config/cors")));

// IMPORTANT: Parse JSON before registering routes
app.use(express.json());

// Routes
const deviceRoutes = require("./routes/deviceRoutes");
const tripRoutes = require("./routes/tripRoutes");
const telemetryRoutes = require("./routes/telemetryRoutes");
const incidentRoutes = require("./routes/incidentRoutes");
const dashboardRoutes = require("./routes/dashboardRoutes");

app.use("/api/devices", deviceRoutes);
app.use("/api/trips", tripRoutes);
app.use("/api/telemetry", telemetryRoutes);
app.use("/api/incidents", incidentRoutes);
app.use("/api/dashboard", dashboardRoutes);

// Health check
app.get("/api/health", (req, res) => {
  res.json({
    success: true,
    message: "Cold Chain API is running",
  });
});

module.exports = app;
