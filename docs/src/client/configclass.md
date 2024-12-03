
# ConfigClass

## Description

The `Config` class is designed to manage configuration files efficiently. It provides functionality to load, modify, and save configuration data, stored as a map associating line numbers with strings. This approach enables precise handling of configuration file contents, ensuring ease of updates and readability.

### Features

- **File Parsing:** Automatically reads and parses a configuration file into memory, mapping line numbers to configuration strings for structured management.
- **Data Access:** Provides methods to retrieve the current configuration or update specific parameters by line number.
- **Error Handling:** Ensures robust error management by integrating with the `ErrorClass`, providing clear messages for issues such as invalid files or empty configurations.
- **File Saving:** Saves the current state of the configuration back to the file system, adding a `.cfg` extension if not already present.

### Functions

- **`getConfig()`**: Retrieves the entire configuration as a map of line numbers to strings.
- **`setConfig(int line, const std::string &param)`**: Updates or adds a configuration parameter at the specified line.
- **`saveConfig()`**: Writes the current configuration back to the file.
- **`parseConfig()`**: Reads and parses the configuration file during initialization.

### Key Variables

- **`_filename`**: Stores the name of the configuration file being managed.
- **`_configData`**: Holds configuration data as a map of line numbers to strings.

---

The `Config` class simplifies the process of managing configuration files, ensuring that applications can handle configuration updates and retrieval efficiently while providing robust error handling mechanisms.
