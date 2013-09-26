package cl.telematic.model;

import javax.persistence.Column;
import javax.persistence.DiscriminatorValue;
import javax.persistence.Entity;
import javax.validation.constraints.NotNull;
import java.util.Calendar;
import java.util.Date;

@Entity
@DiscriminatorValue("DMD")
public class DemandStats extends StatsEntry {

    @NotNull
    @Column(name = "HOUR")
    private Integer hour;

    @NotNull
    @Column(name = "KVA_BLOCK_DEMAND")
    private Long kva_block_demand;

    @NotNull
    @Column(name = "KVAR_IMPORT_BLOCK_DEMAND")
    private Long kvar_import_block_demand;

    @NotNull
    @Column(name = "KW_IMPORT_BLOCK_DEMAND")
    private Long kw_import_block_demand;

    public Integer getHour()
    {
        return hour;
    }

    public Long getKva_block_demand()
    {
        return kva_block_demand;
    }

    public void setKva_block_demand(Long kva_block_demand)
    {
        this.kva_block_demand = kva_block_demand;
    }

    public Long getKvar_import_block_demand()
    {
        return kvar_import_block_demand;
    }

    public void setKvar_import_block_demand(Long kvar_import_block_demand)
    {
        this.kvar_import_block_demand = kvar_import_block_demand;
    }

    public Long getKw_import_block_demand()
    {
        return kw_import_block_demand;
    }

    public void setKw_import_block_demand(Long kw_import_block_demand)
    {
        this.kw_import_block_demand = kw_import_block_demand;
    }

    @Override
    public void setCreatedOn(Date createdOn)
    {
        if (null == createdOn) {
            hour = null;
        } else {
            final Calendar calendar = Calendar.getInstance();
            calendar.setTime(createdOn);
            hour = calendar.get(Calendar.HOUR_OF_DAY);
        }
        super.setCreatedOn(createdOn);
    }
}
