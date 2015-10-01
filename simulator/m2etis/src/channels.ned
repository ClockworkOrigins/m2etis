package m2etis;

// Ethernet channel for SimpleUnderlay
channel simple_m2etis_channel extends ned.DatarateChannel
{
    parameters:
        delay = 30ms;
        datarate = 1000Mbps;
}
