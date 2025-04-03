package com.ice3037.backend.area;

import com.ice3037.backend.area.domain.Area;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

@RequiredArgsConstructor
@Transactional
@Service
public class AreaService {

    private final AreaRepository areaRepository;


    public Long getAreaIdByGps(final Double latitude, final Double longitude) {
        return areaRepository.findAll().stream()
                .filter(area -> isInArea(area, latitude, longitude))
                .findFirst().map(Area::getId).orElse(null);
    }

    private double cross(Double x1, Double y1, Double x2, Double y2, Double x3, Double y3) { // 외적
        return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
    }

    private boolean isInArea(final Area area, final Double latitude, final Double longitude) {
        boolean b1 = cross(area.getLatitude1(), area.getLongitude1(), area.getLatitude2(), area.getLongitude2(), latitude, longitude) >= 0;
        boolean b2 = cross(area.getLatitude2(), area.getLongitude2(), area.getLatitude3(), area.getLongitude3(), latitude, longitude) >= 0;
        boolean b3 = cross(area.getLatitude3(), area.getLongitude3(), area.getLatitude4(), area.getLongitude4(), latitude, longitude) >= 0;
        boolean b4 = cross(area.getLatitude4(), area.getLongitude4(), area.getLatitude1(), area.getLongitude1(), latitude, longitude) >= 0;

        return (b1 && b2 && b3 && b4) || (!b1 && !b2 && !b3 && !b4);
    }

}
