package main

import (
	"fmt"
)

func pulseHigh(t int) {
	for i := 0; i < t; i++ {
		fmt.Print("1,")
	}
}

func pulseLow(t int) {
	for i := 0; i < t; i++ {
		fmt.Print("0,")
	}
}

type Cmd = [12]bool

func cmd(homeCode string, unitCode string,
	cmdCode string) Cmd {
	var result Cmd
	switch homeCode {
	case "b":
		result[0] = true
	case "c":
		result[1] = true
	}
	switch unitCode {
	case "2":
		result[4] = true
	case "3":
		result[5] = true
	}
	result[9] = true
	result[10] = true
	switch cmdCode {
	case "On":
		result[11] = true
	}
	return result
}

const (
	//shortPulse = 320
	shortPulse = 1
	longPulse  = 3 * shortPulse
	//waitTime   = 8000
	waitTime = 25
)

func sendBit(bit bool) {
	pulseHigh(shortPulse)
	pulseLow(longPulse)
	if bit {
		pulseHigh(longPulse)
		pulseLow(shortPulse)
	} else {
		pulseHigh(shortPulse)
		pulseLow(longPulse)
	}
}

func send(title string, cmd Cmd) {
	fmt.Printf("const short %s[126] = {", title)
	//for t := 1; t <= 4; t++ {
	for _, bit := range cmd {
		sendBit(bit)
	}
	pulseHigh(shortPulse)
	pulseLow(longPulse)
	pulseLow(waitTime)
	fmt.Println("0};")
	//}
}

func main() {
	A1_On := cmd("a", "1", "On")
	A1_Off := cmd("a", "1", "Off")
	A2_On := cmd("a", "2", "On")
	A2_Off := cmd("a", "2", "Off")
	A3_On := cmd("a", "3", "On")
	A3_Off := cmd("a", "3", "Off")
	send("a1_on", A1_On)
	send("a1_off", A1_Off)
	send("a2_on", A2_On)
	send("a2_off", A2_Off)
	send("a3_on", A3_On)
	send("a3_off", A3_Off)
}
