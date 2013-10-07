package cl.telematic.rest.domain;

import java.sql.Timestamp;
import java.util.Date;

public class InstantStats {

    private Timestamp createdOn;

    private Long total_kw;

    private Long total_pf;

    private Long v1_voltage;

    private Long v2_voltage;

    private Long v3_voltage;

    public Timestamp getCreatedOn()
    {
        return createdOn;
    }

    public void setCreatedOn(Timestamp createdOn)
    {
        this.createdOn = createdOn;
    }

    public Long getTotal_kw()
    {
        return total_kw;
    }

    public void setTotal_kw(Long total_kw)
    {
        this.total_kw = total_kw;
    }

    public Long getTotal_pf()
    {
        return total_pf;
    }

    public void setTotal_pf(Long total_pf)
    {
        this.total_pf = total_pf;
    }

    public Long getV1_voltage()
    {
        return v1_voltage;
    }

    public void setV1_voltage(Long v1_voltage)
    {
        this.v1_voltage = v1_voltage;
    }

    public Long getV2_voltage()
    {
        return v2_voltage;
    }

    public void setV2_voltage(Long v2_voltage)
    {
        this.v2_voltage = v2_voltage;
    }

    public Long getV3_voltage()
    {
        return v3_voltage;
    }

    public void setV3_voltage(Long v3_voltage)
    {
        this.v3_voltage = v3_voltage;
    }
}
