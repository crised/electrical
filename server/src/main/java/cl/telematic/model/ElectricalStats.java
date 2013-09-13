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
    private Integer activePower1;

    @NotNull
    @Column(name = "ACTIVE_POWER_2")
    private Integer activePower2;

    @NotNull
    @Column(name = "ACTIVE_POWER_3")
    private Integer activePower3;

    @NotNull
    @Column(name = "APPARENT_POWER_1")
    private Integer apparentPower1;

    @NotNull
    @Column(name = "APPARENT_POWER_2")
    private Integer apparentPower2;

    @NotNull
    @Column(name = "APPARENT_POWER_3")
    private Integer apparentPower3;

    @NotNull
    @Column(name = "CURRENT_IN_NEUTRAL")
    private Integer current0;

    @NotNull
    @Column(name = "CURRENT_1")
    private Integer current1;

    @NotNull
    @Column(name = "CURRENT_2")
    private Integer current2;

    @NotNull
    @Column(name = "CURRENT_3")
    private Integer current3;

    @NotNull
    @Column(name = "FREQUENCY")
    private Integer frequency;

    @NotNull
    @Column(name = "REACTIVE_POWER_1")
    private Integer reactivePower1;

    @NotNull
    @Column(name = "REACTIVE_POWER_2")
    private Integer reactivePower2;

    @NotNull
    @Column(name = "REACTIVE_POWER_3")
    private Integer reactivePower3;

    @NotNull
    @Column(name = "VOLTAGE_1")
    private Integer voltage1;

    @NotNull
    @Column(name = "VOLTAGE_1_3")
    private Integer voltage1_3;

    @NotNull
    @Column(name = "VOLTAGE_2")
    private Integer voltage2;

    @NotNull
    @Column(name = "VOLTAGE_2_3")
    private Integer voltage2_3;

    @NotNull
    @Column(name = "VOLTAGE_3")
    private Integer voltage3;

    @NotNull
    @Column(name = "VOLTAGE_3_1")
    private Integer voltage3_1;

    public Integer getActivePower1()
    {
        return activePower1;
    }

    public void setActivePower1(Integer activePower1)
    {
        this.activePower1 = activePower1;
    }

    public Integer getActivePower2()
    {
        return activePower2;
    }

    public void setActivePower2(Integer activePower2)
    {
        this.activePower2 = activePower2;
    }

    public Integer getActivePower3()
    {
        return activePower3;
    }

    public void setActivePower3(Integer activePower3)
    {
        this.activePower3 = activePower3;
    }

    public Integer getApparentPower1()
    {
        return apparentPower1;
    }

    public void setApparentPower1(Integer apparentPower1)
    {
        this.apparentPower1 = apparentPower1;
    }

    public Integer getApparentPower2()
    {
        return apparentPower2;
    }

    public void setApparentPower2(Integer apparentPower2)
    {
        this.apparentPower2 = apparentPower2;
    }

    public Integer getApparentPower3()
    {
        return apparentPower3;
    }

    public void setApparentPower3(Integer apparentPower3)
    {
        this.apparentPower3 = apparentPower3;
    }

    public Integer getCurrent0()
    {
        return current0;
    }

    public void setCurrent0(Integer current0)
    {
        this.current0 = current0;
    }

    public Integer getCurrent1()
    {
        return current1;
    }

    public void setCurrent1(Integer current1)
    {
        this.current1 = current1;
    }

    public Integer getCurrent2()
    {
        return current2;
    }

    public void setCurrent2(Integer current2)
    {
        this.current2 = current2;
    }

    public Integer getCurrent3()
    {
        return current3;
    }

    public void setCurrent3(Integer current3)
    {
        this.current3 = current3;
    }

    public Integer getFrequency()
    {
        return frequency;
    }

    public void setFrequency(Integer frequency)
    {
        this.frequency = frequency;
    }

    public Integer getReactivePower1()
    {
        return reactivePower1;
    }

    public void setReactivePower1(Integer reactivePower1)
    {
        this.reactivePower1 = reactivePower1;
    }

    public Integer getReactivePower2()
    {
        return reactivePower2;
    }

    public void setReactivePower2(Integer reactivePower2)
    {
        this.reactivePower2 = reactivePower2;
    }

    public Integer getReactivePower3()
    {
        return reactivePower3;
    }

    public void setReactivePower3(Integer reactivePower3)
    {
        this.reactivePower3 = reactivePower3;
    }

    public Integer getVoltage1()
    {
        return voltage1;
    }

    public void setVoltage1(Integer voltage1)
    {
        this.voltage1 = voltage1;
    }

    public Integer getVoltage1_3()
    {
        return voltage1_3;
    }

    public void setVoltage1_3(Integer voltage1_3)
    {
        this.voltage1_3 = voltage1_3;
    }

    public Integer getVoltage2()
    {
        return voltage2;
    }

    public void setVoltage2(Integer voltage2)
    {
        this.voltage2 = voltage2;
    }

    public Integer getVoltage2_3()
    {
        return voltage2_3;
    }

    public void setVoltage2_3(Integer voltage2_3)
    {
        this.voltage2_3 = voltage2_3;
    }

    public Integer getVoltage3()
    {
        return voltage3;
    }

    public void setVoltage3(Integer voltage3)
    {
        this.voltage3 = voltage3;
    }

    public Integer getVoltage3_1()
    {
        return voltage3_1;
    }

    public void setVoltage3_1(Integer voltage3_1)
    {
        this.voltage3_1 = voltage3_1;
    }
}
