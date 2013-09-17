package cl.telematic.model;

import javax.persistence.Column;
import javax.persistence.DiscriminatorColumn;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.Inheritance;
import javax.persistence.InheritanceType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.validation.constraints.NotNull;
import java.io.Serializable;
import java.util.Date;

@Entity
@Inheritance(strategy = InheritanceType.SINGLE_TABLE)
@DiscriminatorColumn(name = "STATS_TYPE")
@Table(name = "STATS_ENTRY")
public abstract class StatsEntry implements Serializable {

    @NotNull
    @Column(name = "CREATED_DATE", nullable = false)
    @Temporal(TemporalType.TIMESTAMP)
    private Date createdOn;

    @NotNull
    @ManyToOne(optional = false)
    @JoinColumn(name = "DEVICE_ID", nullable = false)
    private Device device;

    @Id
    @GeneratedValue
    private Long id;

    public Date getCreatedOn()
    {
        return createdOn;
    }

    public void setCreatedOn(Date createdOn)
    {
        this.createdOn = createdOn;
    }

    public Device getDevice()
    {
        return device;
    }

    public void setDevice(Device device)
    {
        this.device = device;
    }

    public Long getId()
    {
        return id;
    }

    public void setId(Long id)
    {
        this.id = id;
    }

    @Override
    public boolean equals(Object o)
    {
        if (this == o) {
            return true;
        }
        if (!(o instanceof StatsEntry)) {
            return false;
        }

        StatsEntry that = (StatsEntry) o;

        return !(getId() != null ? !getId().equals(that.getId()) : that.getId() != null);
    }

    @Override
    public int hashCode()
    {
        return getId() != null ? getId().hashCode() : 0;
    }
}
