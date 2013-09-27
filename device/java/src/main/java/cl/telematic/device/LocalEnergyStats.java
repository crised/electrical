package cl.telematic.device;

import cl.telematic.rest.domain.EnergyStats;
import org.codehaus.jackson.annotate.JsonIgnoreProperties;

@JsonIgnoreProperties({"id"})
public class LocalEnergyStats extends EnergyStats implements Identifiable {

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
