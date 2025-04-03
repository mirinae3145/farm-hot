package com.ice3037.backend.farmer.domain;

import jakarta.persistence.*;
import lombok.*;
import lombok.extern.slf4j.Slf4j;

@Entity
@Slf4j
@Getter
@Builder(toBuilder = true)
@AllArgsConstructor(access = AccessLevel.PRIVATE)
@NoArgsConstructor(access = AccessLevel.PROTECTED)
public class Farmer {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(nullable = false)
    private String name;

    @Column(nullable = false)
    private boolean isTemperatureNormal;

    @Column
    private Long areaId;

    public Farmer update(final boolean isTemperatureNormal, final Long areaId) {
        this.isTemperatureNormal = isTemperatureNormal;
        this.areaId = areaId;
        return this;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Farmer farmer = (Farmer) o;
        return id.equals(farmer.id);
    }

    @Override
    public int hashCode() {
        return id.hashCode();
    }

}
