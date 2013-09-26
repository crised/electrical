package cl.telematic.rest.domain;

public class DeviceStats {

    private Long activePowerTotalMax18_23;

    private Long deviceId;

    private String deviceName;

    private Aggregate kva_block_demand;

    private Aggregate kvar_import_block_demand;

    private Aggregate kw_import_block_demand;

    private Long kwh_import;

    private Long kwh_import_l1;

    private Long kwh_import_l2;

    private Long kwh_import_l3;

    private Long total_kw;

    private Long total_pf;

    private Long v1_voltage;

    private Long v2_voltage;

    private Long v3_voltage;

    public Long getActivePowerTotalMax18_23()
    {
        return activePowerTotalMax18_23;
    }

    public void setActivePowerTotalMax18_23(Long activePowerTotalMax18_23)
    {
        this.activePowerTotalMax18_23 = activePowerTotalMax18_23;
    }

    public Long getDeviceId()
    {
        return deviceId;
    }

    public void setDeviceId(Long deviceId)
    {
        this.deviceId = deviceId;
    }

    public String getDeviceName()
    {
        return deviceName;
    }

    public void setDeviceName(String deviceName)
    {
        this.deviceName = deviceName;
    }

    public Aggregate getKva_block_demand()
    {
        return kva_block_demand;
    }

    public void setKva_block_demand(Aggregate kva_block_demand)
    {
        this.kva_block_demand = kva_block_demand;
    }

    public Aggregate getKvar_import_block_demand()
    {
        return kvar_import_block_demand;
    }

    public void setKvar_import_block_demand(Aggregate kvar_import_block_demand)
    {
        this.kvar_import_block_demand = kvar_import_block_demand;
    }

    public Aggregate getKw_import_block_demand()
    {
        return kw_import_block_demand;
    }

    public void setKw_import_block_demand(Aggregate kw_import_block_demand)
    {
        this.kw_import_block_demand = kw_import_block_demand;
    }

    public Long getKwh_import()
    {
        return kwh_import;
    }

    public void setKwh_import(Long kwh_import)
    {
        this.kwh_import = kwh_import;
    }

    public Long getKwh_import_l1()
    {
        return kwh_import_l1;
    }

    public void setKwh_import_l1(Long kwh_import_l1)
    {
        this.kwh_import_l1 = kwh_import_l1;
    }

    public Long getKwh_import_l2()
    {
        return kwh_import_l2;
    }

    public void setKwh_import_l2(Long kwh_import_l2)
    {
        this.kwh_import_l2 = kwh_import_l2;
    }

    public Long getKwh_import_l3()
    {
        return kwh_import_l3;
    }

    public void setKwh_import_l3(Long kwh_import_l3)
    {
        this.kwh_import_l3 = kwh_import_l3;
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
