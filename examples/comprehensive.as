mission ComprehensiveTest launch {
    
    $$ ==========================================
    $$ 1. VARIABLE DECLARATION & ASSIGNMENT
    $$ ==========================================
    
    telemetry count x.
    telemetry count y.
    telemetry count sum.
    telemetry count product.
    telemetry count counter.
    telemetry count result.
    
    $$ Initialize variables with values
    x := 5.
    y := 3.
    
    transmit x.
    transmit y.
    
    
    $$ ==========================================
    $$ 2. EXPRESSION EVALUATION
    $$ ==========================================
    
    $$ Basic arithmetic
    sum := x add y.
    transmit sum.
    
    $$ Multiplication and addition (order of operations)
    product := x mul y.
    transmit product.
    
    $$ Complex expression: (x + y) * 2
    result := x add y mul 2.
    transmit result.
    
    $$ Division
    result := product divide x.
    transmit result.
    
    $$ Subtraction chain
    result := sum minus y.
    transmit result.
    
    
    $$ ==========================================
    $$ 3. CONDITIONAL STATEMENTS
    $$ ==========================================
    
    $$ Simple if statement
    verify (x > 0) {
        result := 100.
        transmit result.
    } otherwise {
        result := 0.
        transmit result.
    }
    
    $$ If comparison
    verify (x > y) {
        result := 1.
        transmit result.
    } else_verify (x == y) {
        result := 0.
        transmit result.
    } else_verify (x < y) {
        result := -1.
        transmit result.
    }
    
    $$ Range check
    verify (sum > 5) {
        result := 888.
        transmit result.
    } otherwise {
        result := 0.
        transmit result.
    }
    
    
    $$ ==========================================
    $$ 4. LOOPS
    $$ ==========================================
    
    $$ Loop with counter - orbit (while condition)
    counter := 0.
    orbit (counter < 5) {
        transmit counter.
        counter := counter add 1.
    }
    
    $$ Loop with range - orbit times (for loop style)
    orbit times (counter : 0 : 3) {
        result := counter mul x.
        transmit result.
    }
    
    
    $$ ==========================================
    $$ 5. VARIABLE REASSIGNMENT IN LOOPS
    $$ ==========================================
    
    result := 1.
    counter := 1.
    
    $$ Loop to calculate factorial of 5
    orbit (counter < 6) {
        result := result mul counter.
        counter := counter add 1.
    }
    
    $$ Final result: 5! = 120
    transmit result.
    
    
    $$ ==========================================
    $$ 6. COMPLEX SCENARIO: FIBONACCI SEQUENCE
    $$ ==========================================
    
    telemetry count a.
    telemetry count b.
    telemetry count c.
    
    a := 0.
    b := 1.
    counter := 0.
    
    transmit a.
    transmit b.
    
    $$ Generate first 8 Fibonacci numbers
    orbit (counter < 8) {
        c := a add b.
        transmit c.
        a := b.
        b := c.
        counter := counter add 1.
    }
    
    
    $$ ==========================================
    $$ 7. LOGICAL COMPARISONS IN CONDITIONS
    $$ ==========================================
    
    telemetry count min_val.
    telemetry count max_val.
    telemetry count status.
    
    min_val := 5.
    max_val := 15.
    status := 0.
    
    verify (x == min_val) {
        status := 501.
        transmit status.
    } otherwise {
        status := 502.
        transmit status.
    }
    
    verify (y <= max_val) {
        status := 601.
        transmit status.
    } otherwise {
        status := 602.
        transmit status.
    }
    
    verify (sum >= 8) {
        status := 701.
        transmit status.
    } otherwise {
        status := 702.
        transmit status.
    }
    
    verify (product != 0) {
        status := 801.
        transmit status.
    } otherwise {
        status := 802.
        transmit status.
    }
    
    $$ ==========================================
    $$ 8. FINAL SUMMARY
    $$ ==========================================
    
    status := 9999.
    transmit status.
    
} success
