#  Data Dict

![Gnomon Data Dict illustration Image](../../_static/user_guide/data_dict_image.png){width=200px class="sd-rounded-1 sd-shadow-sm" align=center}


A Data Dict is a versatile form representing a dictionary.
The **key** is a string and the **value** can be of the following types: int, string, bool, float or a numpy array.
If it's a numpy array the dimension has to be <= 4 and the type needs to be NPY_INT or NPY_DOUBLE. 


## Default implementation
This form is implemented in the package [gnomon_package_data](../../plugins/packages/gnomon_package_data) with a **reader**, a **writer** and a **visualization**.

The reader of data dict form is **gnomonDataDictReaderJson**. It loads a Json file as a dictionary.  
This reader supports only one extension `json`.

## Workspaces using Data Dict

### Producers
- [Data Browsing](../workspaces/data_browsing)
- [Image Registration](../workspaces/image_registration)
- [Python Algorithm](../workspaces/python_algorithm)

### Consumers
- [Cell Image Tracking](../workspaces/cell_image_tracking)
- [Image Registration](../workspaces/image_registration)
- [Python Algorithm](../workspaces/python_algorithm)



