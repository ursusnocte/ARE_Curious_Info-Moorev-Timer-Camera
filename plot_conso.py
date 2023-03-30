import pandas as pd
import matplotlib.pyplot as plt

# Read the data
df_esp32 = pd.read_csv("esp32conso.xls.csv", sep=";")
df_nano = pd.read_csv("arduinoconso.xls.csv", sep=";")

# Convert data to float
data_esp32 = [float(x) for x in df_esp32["Value"]]
data_arduino = [float(x) for x in df_nano["Value"]]

time_esp32 = pd.to_datetime(df_esp32["Time"])
time_arduino = pd.to_datetime(df_nano["Time"])

# Print mean and standard deviation
print("ESP32 mean: ", sum(data_esp32)/len(data_esp32))
print("ESP32 standard deviation: ", (sum([(x - sum(data_esp32)/len(data_esp32))**2 for x in data_esp32])/len(data_esp32))**0.5)
print("Arduino mean: ", sum(data_arduino)/len(data_arduino))
print("Arduino standard deviation: ", (sum([(x - sum(data_arduino)/len(data_arduino))**2 for x in data_arduino])/len(data_arduino))**0.5)

# Print total time and total consumption 
diff_time_esp32 = time_esp32[len(time_esp32)-1] - time_esp32[0]
diff_time_arduino = time_arduino[len(time_arduino)-1] - time_arduino[0]
print("ESP32 total time: ", divmod(diff_time_esp32.total_seconds(), 60)[0], "min", divmod(diff_time_esp32.total_seconds(), 60)[1], "s")
print("ESP32 total consumption: ", round(sum(data_esp32),2), "mA")
print("Arduino total time: ", divmod(diff_time_arduino.total_seconds(), 60)[0], "min", divmod(diff_time_arduino.total_seconds(), 60)[1], "s")
print("Arduino total consumption: ", round(sum(data_arduino),2), "mA")

# Plot the data
plt.plot(data_esp32, label="ESP32 consumption", color="red")
plt.plot(data_arduino, label="Arduino consumption", color="blue")
plt.legend()
plt.xlabel("Arbitrary time unit (1 min between each activation)")
plt.ylabel("Consumption (mA)")
plt.show()