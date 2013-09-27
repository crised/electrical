package cl.telematic.rest.domain;

import java.util.Date;

public class EnergyStats {

    private Date createdOn;

    private Long kwh_import;

    private Long kwh_import_l1;

    private Long kwh_import_l2;

    private Long kwh_import_l3;

    public Date getCreatedOn()
    {
        return createdOn;
    }

    public void setCreatedOn(Date createdOn)
    {
        this.createdOn = createdOn;
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
}

