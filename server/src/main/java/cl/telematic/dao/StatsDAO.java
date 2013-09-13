package cl.telematic.dao;

import cl.telematic.model.ActiveTotalEnergyStats;
import cl.telematic.model.ElectricalStats;

import javax.annotation.Nonnull;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;

@Stateless
public class StatsDAO {

    @PersistenceContext
    private EntityManager entityManager;

    @Nonnull
    public ActiveTotalEnergyStats save(@Nonnull ActiveTotalEnergyStats stats)
    {
        return entityManager.merge(stats);
    }

    @Nonnull
    public ElectricalStats save(@Nonnull ElectricalStats stats)
    {
        return entityManager.merge(stats);
    }
}
