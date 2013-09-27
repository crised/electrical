package cl.telematic.dao;

import cl.telematic.rest.domain.InstantStats;

import javax.annotation.Nullable;
import javax.ejb.Singleton;
import java.util.HashMap;
import java.util.Map;

@Singleton
public class CacheManager {

    private final Map<Long, InstantStats> instantStatsCache = new HashMap<Long, InstantStats>();

    @Nullable
    public InstantStats getInstantStats(@Nullable Long deviceId)
    {
        return instantStatsCache.get(deviceId);
    }

    public void putInstantStats(@Nullable Long deviceId, @Nullable InstantStats stats)
    {
        instantStatsCache.put(deviceId, stats);
    }
}
