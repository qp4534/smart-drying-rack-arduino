// CDS 및 포토센서 값 읽기
for (i = 0; i < 6; i++) {
    cds[i] = analogRead(A0 + i); 
    // 디버깅용 출력
    Serial.print(cds[i]);
    Serial.print(" ");
}
Serial.println(" ");

// 1. 조도센서(CDS) 최댓값 및 위치 찾기
max_cds = 0;
max_cds_num = 0; // 초기화 추가
for (i = 0; i < 6; i++) { // 주의: 센서가 6개이므로 10이 아닌 6이어야 함
    if (max_cds < cds[i]) {
        max_cds_num = i;
        max_cds = cds[i];
    }
}

mov = max_cds_num - pos;

// 2. 리니어 모터 이동 로직 (조도값이 850 이상일 때만 동작)
if (max_cds > 850) {
    if (mov > 0) { // 양의 방향으로 움직여야 한다면
        // 역방향 모터 회전 설정
        HCMotor.Direction(0, REVERSE); // 기구부 설계에 따라 FORWARD/REVERSE 변경 필요
        Speed = 3;
        HCMotor.DutyCycle(0, Speed);
        
        // 목표 위치의 포토센서 값이 950 이상이 될 때까지 대기
        while (analogRead(A10 + max_cds_num) <= 950) {
            delay(10); // 안정성을 위한 짧은 대기
        }
        
    } else if (mov < 0) { // 음의 방향으로 움직여야 한다면
        // 정방향 모터 회전 설정
        HCMotor.Direction(0, FORWARD); // 기구부 설계에 따라 FORWARD/REVERSE 변경 필요
        Speed = 3;
        HCMotor.DutyCycle(0, Speed);
        
        // 목표 위치의 포토센서 값이 950 이상이 될 때까지 대기
        while (analogRead(A10 + max_cds_num) <= 950) {
            delay(10);
        }
    }
    
    // 3. 모터 정지 및 현재 위치 업데이트
    Speed = 0;
    HCMotor.DutyCycle(0, Speed);
    pos = max_cds_num; // 위치 업데이트
} else {
    // 조도값이 850 이하일 때는 모터 정지 상태 유지
    Speed = 0;
    HCMotor.DutyCycle(0, Speed);
}
