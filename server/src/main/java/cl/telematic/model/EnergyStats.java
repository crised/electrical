package cl.telematic.model;

import javax.persistence.Column;
import javax.persistence.DiscriminatorValue;
import javax.persistence.Entity;
import javax.validation.constraints.NotNull;

@Entity
@DiscriminatorValue("ENG")
public class EnergyStats extends StatsEntry {

    @NotNull
    @Column(name = "KWH_IMPORT")
    private Long kwh_import;

    @NotNull
    @Column(name = "KWH_IMPORT_L1")
    private Long kwh_import_l1;

    @NotNull
    @Column(name = "KWH_IMPORT_L2")
    private Long kwh_import_l2;

    @NotNull
    @Column(name = "KWH_IMPORT_L3")
    private Long kwh_import_l3;

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
