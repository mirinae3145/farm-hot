package com.ice3037.backend.farmer;

import com.ice3037.backend.farmer.dto.FarmerSaveRequest;
import com.ice3037.backend.farmer.dto.FarmerSavedResponse;
import com.ice3037.backend.farmer.dto.FarmerUpdateRequest;
import lombok.RequiredArgsConstructor;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

@RequiredArgsConstructor
@RestController
@RequestMapping("/farmer")
public class FarmerController {

    private final FarmerService farmerService;

    @PostMapping
    public ResponseEntity<HttpStatus> postFarmer(@RequestBody final FarmerSaveRequest farmerSaveRequest) {
        farmerService.createFarmer(farmerSaveRequest.name());
        return new ResponseEntity<>(HttpStatus.CREATED);
    }

    @GetMapping("/{id}")
    public ResponseEntity<FarmerSavedResponse> getFarmer(@PathVariable final Long id) {
        return ResponseEntity.ok(
                FarmerSavedResponse.of(farmerService.readFarmer(id))
        );
    }

    @GetMapping
    public ResponseEntity<FarmerSavedResponse> getFarmer(@RequestParam String name) {
        return ResponseEntity.ok(
                FarmerSavedResponse.of(farmerService.readFarmerByName(name))
        );
    }

    @PatchMapping("/{id}")
    public ResponseEntity<HttpStatus> updateFarmer(@PathVariable final Long id, @RequestBody final FarmerUpdateRequest farmerUpdateRequest) {
        farmerService.updateFarmer(
                id,
                farmerUpdateRequest.isTemperatureNormal(),
                farmerUpdateRequest.latitude(),
                farmerUpdateRequest.longitude());
        return new ResponseEntity<>(HttpStatus.OK);
    }


}
