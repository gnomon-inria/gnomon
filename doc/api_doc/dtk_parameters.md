# `dtkcore` Parameters

The `dtkcore` Python module provides for a variety of classes for different types notably for
integers, real numbers, character strings, boolean and so on:

## String

![](../_static/api_doc/d_string.png)

```{py:class} d_string(label: str, default_value: float, doc: str)

```

## Numeric

![](../_static/api_doc/d_int.png)

```{py:class} d_uchar(label: str, default_value: int, min: int, max: int, doc: str)

```
```{py:class} d_char(label: str, default_value: int, min: int, max: int, doc: str)

```
```{py:class} d_uint(label: str, default_value: int, min: int, max: int, doc: str)

```
```{py:class} d_int(label: str, default_value: int, min: int, max: int, doc: str)

```

![](../_static/api_doc/d_real.png)
```{py:class} d_real(label: str, default_value: float, min: float, max: float, decimals: int, doc: str)

```

![](../_static/api_doc/d_bool.png)
```{py:class} d_bool(label: str, default_value: float, doc: str)

```

## Numeric Range

![](../_static/api_doc/d_range_real.png)


```{py:class} d_range_uchar(label: str, default_value: dtkcore.array_uchar_2, min: int, max: int, doc: str)

```
```{py:class} array_uchar_2(arr: tuple[int, int])

```
```{py:class} d_range_char(label: str, default_value: dtkcore.array_char_2, min: int, max: int, doc: str)

```
```{py:class} array_char_2(arr: tuple[int, int])

```
```{py:class} d_range_uint(label: str, default_value: dtkcore.array_uint_2, min: int, max: int, doc: str)

```
```{py:class} array_uint_2(arr: tuple[int, int])

```
```{py:class} d_range_int(label: str, default_value: dtkcore.array_int_2, min: int, max: int, doc: str)

```
```{py:class} array_int_2(arr: tuple[int, int])

```
```{py:class} d_range_real(label: str, default_value: dtkcore.array_real_2, min: float, max: float, doc: str)

```
```{py:class} array_real_2(arr: tuple[int, int])

```

## List (combobox)

![](../_static/api_doc/d_inliststring.png)

```{py:class} d_inliststring(label: str, default_value: str, available_values: list[str], doc: str)

```

```{py:class} d_inlistint(label: str, default_value: int, available_values: list[int], doc: str)

```
```{py:class} d_inlistreal(label: str, default_value: float, available_values: list[float], doc: str)

```
## Multiple choice

![](../_static/api_doc/d_inliststringlist.png)

```{py:class} d_inliststringlist(label: str, selected_values: list[str], available_values: list[str], doc: str)

```

## Path

![](../_static/api_doc/d_path.png)

```{py:class} d_path(label: str, path: str, filters: list[str], doc: str)

```
