package cl.telematic.rest.domain;

import java.sql.Timestamp;
import java.util.Date;

public class DemandStats {

    private Timestamp createdOn;

    private Long kva_block_demand;

    private Long kvar_import_block_demand;

    private Long kw_import_block_demand;

    public Timestamp getCreatedOn()
    {
        return createdOn;
    }

    public void setCreatedOn(Timestamp createdOn)
    {
        this.createdOn = createdOn;
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
}

