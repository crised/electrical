package cl.telematic.device;

import cl.telematic.rest.domain.InstantStats;

public class LocalInstantStats extends InstantStats implements Identifiable {

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
