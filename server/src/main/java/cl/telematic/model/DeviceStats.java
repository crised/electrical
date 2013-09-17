package cl.telematic.model;

import java.io.Serializable;

public class DeviceStats implements Serializable {

    public Aggregate activePowerTotal;

    public Aggregate activePower1;

    public Aggregate activePower2;

    public Aggregate activePower3;

    public Aggregate apparentPowerTotal;

    public Aggregate apparentPower1;

    public Aggregate apparentPower2;

    public Aggregate apparentPower3;

    public Aggregate current0;

    public Aggregate current1;

    public Aggregate current2;

    public Aggregate current3;

    public Long deviceId;

    public String deviceName;

    public Double energy;

    public Aggregate frequency1;

    public Aggregate frequency2;

    public Aggregate frequency3;

    public Double activePowerTotalMax18_23;

    public Double powerFactor;

    public Aggregate powerFactor1;

    public Aggregate powerFactor2;

    public Aggregate powerFactor3;

    public Aggregate reactivePowerTotal;

    public Aggregate reactivePower1;

    public Aggregate reactivePower2;

    public Aggregate reactivePower3;

    public Aggregate voltage1;

    public Aggregate voltage1_3;

    public Aggregate voltage2;

    public Aggregate voltage2_3;

    public Aggregate voltage3;

    public Aggregate voltage3_1;

    public static class Aggregate {

        public Number avg;

        public Number max;

        public Number min;

        public Aggregate(Number avg, Number max, Number min)
        {
            this.avg = avg;
            this.max = max;
            this.min = min;
        }
    }
}
