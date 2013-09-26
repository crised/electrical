package cl.telematic.device;

import cl.telematic.rest.domain.DemandStats;

public class LocalDemandStats extends DemandStats implements Identifiable {

    private Long id;

    public Long getId()
    {
        return id;
    }

    public void setId(Long id)
    {
        this.id = id;
    }
}
