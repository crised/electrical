package cl.telematic.model;

import javax.persistence.Column;
import javax.persistence.DiscriminatorValue;
import javax.persistence.Entity;
import javax.validation.constraints.NotNull;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
@Entity
@DiscriminatorValue("ELE")
public class ElectricalStats extends StatsEntry {

    @NotNull
    @Column(name = "ACTIVE_POWER_1")
    private Double activePower1;

    @NotNull
    @Column(name = "ACTIVE_POWER_2")
    private Double activePower2;

    @NotNull
    @Column(name = "ACTIVE_POWER_3")
    private Double activePower3;

    @NotNull
    @Column(name = "ACTIVE_POWER_TOTAL")
    private Double activePowerTotal;

    @NotNull
    @Column(name = "APPARENT_POWER_1")
    private Double apparentPower1;

    @NotNull
    @Column(name = "APPARENT_POWER_2")
    private Double apparentPower2;

    @NotNull
    @Column(name = "APPARENT_POWER_3")
    private Double apparentPower3;

    @NotNull
    @Column(name = "APPARENT_POWER_TOTAL")
    private Double apparentPowerTotal;

    @NotNull
    @Column(name = "CURRENT_IN_NEUTRAL")
    private Double current0;

    @NotNull
    @Column(name = "CURRENT_1")
    private Double current1;

    @NotNull
    @Column(name = "CURRENT_2")
    private Double current2;

    @NotNull
    @Column(name = "CURRENT_3")
    private Double current3;

    @NotNull
    @Column(name = "FREQUENCY_1")
    private Double frequency1;

    @NotNull
    @Column(name = "FREQUENCY_2")
    private Double frequency2;

    @NotNull
    @Column(name = "FREQUENCY_3")
    private Double frequency3;

    @NotNull
    @Column(name = "HOUR")
    private Integer hour;

    @NotNull
    @Column(name = "REACTIVE_POWER_1")
    private Double reactivePower1;

    @NotNull
    @Column(name = "REACTIVE_POWER_2")
    private Double reactivePower2;

    @NotNull
    @Column(name = "REACTIVE_POWER_3")
    private Double reactivePower3;

    @NotNull
    @Column(name = "REACTIVE_POWER_TOTAL")
    private Double reactivePowerTotal;

    @NotNull
    @Column(name = "VOLTAGE_1")
    private Double voltage1;

    @NotNull
    @Column(name = "VOLTAGE_1_3")
    private Double voltage1_3;

    @NotNull
    @Column(name = "VOLTAGE_2")
    private Double voltage2;

    @NotNull
    @Column(name = "VOLTAGE_2_3")
    private Double voltage2_3;

    @NotNull
    @Column(name = "VOLTAGE_3")
    private Double voltage3;

    @NotNull
    @Column(name = "VOLTAGE_3_1")
    private Double voltage3_1;

    public Double getActivePower1()
    {
        return activePower1;
    }

    public void setActivePower1(Double activePower1)
    {
        this.activePower1 = activePower1;
    }

    public Double getActivePower2()
    {
        return activePower2;
    }

    public void setActivePower2(Double activePower2)
    {
        this.activePower2 = activePower2;
    }

    public Double getActivePower3()
    {
        return activePower3;
    }

    public void setActivePower3(Double activePower3)
    {
        this.activePower3 = activePower3;
    }

    public Double getActivePowerTotal()
    {
        return activePowerTotal;
    }

    public void setActivePowerTotal(Double activePowerTotal)
    {
        this.activePowerTotal = activePowerTotal;
    }

    public Double getApparentPower1()
    {
        return apparentPower1;
    }

    public void setApparentPower1(Double apparentPower1)
    {
        this.apparentPower1 = apparentPower1;
    }

    public Double getApparentPower2()
    {
        return apparentPower2;
    }

    public void setApparentPower2(Double apparentPower2)
    {
        this.apparentPower2 = apparentPower2;
    }

    public Double getApparentPower3()
    {
        return apparentPower3;
    }

    public void setApparentPower3(Double apparentPower3)
    {
        this.apparentPower3 = apparentPower3;
    }

    public Double getApparentPowerTotal()
    {
        return apparentPowerTotal;
    }

    public void setApparentPowerTotal(Double apparentPowerTotal)
    {
        this.apparentPowerTotal = apparentPowerTotal;
    }

    public Double getCurrent0()
    {
        return current0;
    }

    public void setCurrent0(Double current0)
    {
        this.current0 = current0;
    }

    public Double getCurrent1()
    {
        return current1;
    }

    public void setCurrent1(Double current1)
    {
        this.current1 = current1;
    }

    public Double getCurrent2()
    {
        return current2;
    }

    public void setCurrent2(Double current2)
    {
        this.current2 = current2;
    }

    public Double getCurrent3()
    {
        return current3;
    }

    public void setCurrent3(Double current3)
    {
        this.current3 = current3;
    }

    public Double getFrequency1()
    {
        return frequency1;
    }

    public void setFrequency1(Double frequency1)
    {
        this.frequency1 = frequency1;
    }

    public Double getFrequency2()
    {
        return frequency2;
    }

    public void setFrequency2(Double frequency2)
    {
        this.frequency2 = frequency2;
    }

    public Double getFrequency3()
    {
        return frequency3;
    }

    public void setFrequency3(Double frequency3)
    {
        this.frequency3 = frequency3;
    }

    public Integer getHour()
    {
        return hour;
    }

    public void setHour(Integer hour)
    {
        this.hour = hour;
    }

    public Double getReactivePower1()
    {
        return reactivePower1;
    }

    public void setReactivePower1(Double reactivePower1)
    {
        this.reactivePower1 = reactivePower1;
    }

    public Double getReactivePower2()
    {
        return reactivePower2;
    }

    public void setReactivePower2(Double reactivePower2)
    {
        this.reactivePower2 = reactivePower2;
    }

    public Double getReactivePower3()
    {
        return reactivePower3;
    }

    public void setReactivePower3(Double reactivePower3)
    {
        this.reactivePower3 = reactivePower3;
    }

    public Double getReactivePowerTotal()
    {
        return reactivePowerTotal;
    }

    public void setReactivePowerTotal(Double reactivePowerTotal)
    {
        this.reactivePowerTotal = reactivePowerTotal;
    }

    public Double getVoltage1()
    {
        return voltage1;
    }

    public void setVoltage1(Double voltage1)
    {
        this.voltage1 = voltage1;
    }

    public Double getVoltage1_3()
    {
        return voltage1_3;
    }

    public void setVoltage1_3(Double voltage1_3)
    {
        this.voltage1_3 = voltage1_3;
    }

    public Double getVoltage2()
    {
        return voltage2;
    }

    public void setVoltage2(Double voltage2)
    {
        this.voltage2 = voltage2;
    }

    public Double getVoltage2_3()
    {
        return voltage2_3;
    }

    public void setVoltage2_3(Double voltage2_3)
    {
        this.voltage2_3 = voltage2_3;
    }

    public Double getVoltage3()
    {
        return voltage3;
    }

    public void setVoltage3(Double voltage3)
    {
        this.voltage3 = voltage3;
    }

    public Double getVoltage3_1()
    {
        return voltage3_1;
    }

    public void setVoltage3_1(Double voltage3_1)
    {
        this.voltage3_1 = voltage3_1;
    }
}
