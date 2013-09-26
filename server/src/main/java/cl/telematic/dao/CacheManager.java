package cl.telematic.dao;

import cl.telematic.rest.domain.InstantStats;

import javax.annotation.Nullable;
import javax.ejb.Singleton;
import java.util.HashMap;
import java.util.Map;

@Singleton
public class CacheManager {

    private final Map<Long, InstantStats> activeEnergyStatsCache = new HashMap<Long, InstantStats>();

    @Nullable
    public InstantStats getActiveEnergyStats(@Nullable Long deviceId)
    {
        return activeEnergyStatsCache.get(deviceId);
    }

    public void putActiveEnergyStats(@Nullable Long deviceId, @Nullable InstantStats stats)
    {
        activeEnergyStatsCache.put(deviceId, stats);
    }
}
