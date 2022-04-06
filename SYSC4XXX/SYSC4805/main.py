import sys
from time import sleep
import sim

sharedLibLocation = "C:/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/remoteApiBindings/lib/lib/Windows"
sys.path.append(sharedLibLocation)
serverPort = 19999
baseSpeed = -1
minorTurnMod = 0.6
majorTurnMod = 0.05


def readSensors(sensors_in):
    # These are all blocking reads, but I just didn't bother with the streaming reads
    results = [False, False, False]
    resL, _, imageL = sim.simxGetVisionSensorImage(clientID, sensors_in[0], 0, sim.simx_opmode_blocking)
    resM, _, imageM = sim.simxGetVisionSensorImage(clientID, sensors_in[1], 0, sim.simx_opmode_blocking)
    resR, _, imageR = sim.simxGetVisionSensorImage(clientID, sensors_in[2], 0, sim.simx_opmode_blocking)
    if resL == sim.simx_return_ok and resM == sim.simx_return_ok and resR == sim.simx_return_ok:
        results[0] = imageL[11] < 0.3
        results[1] = imageM[11] < 0.3
        results[2] = imageR[11] < 0.3
        # print("Sensors Read!")
    return results


clientID = sim.simxStart('127.0.0.1', serverPort, True, True, 5000, 5)
if clientID != -1:
    print("Connected to sim!")
    _, sensorLHandle = sim.simxGetObjectHandle(clientID, 'Vision_Sensor_L', sim.simx_opmode_oneshot_wait)
    _, sensorMHandle = sim.simxGetObjectHandle(clientID, 'Vision_Sensor_M', sim.simx_opmode_oneshot_wait)
    _, sensorRHandle = sim.simxGetObjectHandle(clientID, 'Vision_Sensor_R', sim.simx_opmode_oneshot_wait)

    _, MotorRHandle = sim.simxGetObjectHandle(clientID, 'Right_Motor', sim.simx_opmode_oneshot_wait)
    _, MotorLHandle = sim.simxGetObjectHandle(clientID, 'Left_Motor', sim.simx_opmode_oneshot_wait)

    sensors = [sensorLHandle, sensorMHandle, sensorRHandle]

    sim.simxStartSimulation(clientID, sim.simx_opmode_oneshot)
    while sim.simxGetConnectionId(clientID) != -1:
        leftV = baseSpeed
        rightV = baseSpeed
        currentScan = readSensors(sensors)
        if currentScan[0]:
            leftV *= minorTurnMod
            if not currentScan[1]:
                leftV *= majorTurnMod
        if currentScan[2]:
            rightV *= minorTurnMod
            if not currentScan[1]:
                rightV *= majorTurnMod
        _ = sim.simxSetJointTargetVelocity(clientID, MotorLHandle, leftV, sim.simx_opmode_oneshot)
        _ = sim.simxSetJointTargetVelocity(clientID, MotorRHandle, rightV, sim.simx_opmode_oneshot)
        # sleep(0.001)
    sim.simxFinish(clientID)
    print("Job's done.")

